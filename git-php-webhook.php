<?php
/** 
  * This script is for easily deploying updates to Github repos to your local server. It will automatically git clone or 
  * git pull in your repo directory every time an update is pushed to your $BRANCH (configured below).
  * 
  * Read more about how to use this script at http://behindcompanies.com/2014/01/a-simple-script-for-deploying-code-with-githubs-webhooks/
  * 
  * INSTRUCTIONS:
  * 1. Edit the variables below
  * 2. Upload this script to your server somewhere it can be publicly accessed
  * 3. Make sure the apache user owns this script (e.g., sudo chown www-data:www-data webhook.php)
  * 4. (optional) If the repo already exists on the server, make sure the same apache user from step 3 also owns that 
  *    directory (i.e., sudo chown -R www-data:www-data)
  * 5. Go into your Github Repo > Settings > Service Hooks > WebHook URLs and add the public URL 
  *    (e.g., http://example.com/webhook.php)
  *
**/

// Set Variables
$LOCAL_ROOT         = "/afs/athena.mit.edu/user/m/f/mfe/web_scripts";
$LOCAL_REPO_NAME    = "portfolio";
$LOCAL_REPO         = "{$LOCAL_ROOT}/{$LOCAL_REPO_NAME}";
$REMOTE_REPO        = "git@github.com:mfe7/portfolio.git";
$BRANCH             = "master";

if ($_SERVER['HTTP_X_GITHUB_EVENT'] == 'push') {
  // Only respond to POST requests from Github
    print("got push from github."); 
    if( file_exists($LOCAL_REPO) ) {
        print("repo exists.");
    // If there is already a repo, just run a git pull to grab the latest changes
        shell_exec("cd {$LOCAL_REPO} && git pull");
        die("done " . mktime());
  }
}

?>
