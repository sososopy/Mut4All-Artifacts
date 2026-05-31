//source file
#include "../include/replace_template_specialization_return_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("specializedFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() && FD->hasBody()) {
        //Get the source code text of target node
        auto body = FD->getBody();
        if (body) {
          //Perform mutation on the source code text by applying string replacement
          for (auto it = body->child_begin(); it != body->child_end(); ++it) {
            if (auto returnStmt = dyn_cast<clang::ReturnStmt>(*it)) {
              auto returnRange = returnStmt->getSourceRange();
              std::string mutatedReturn = "return some_undeclared_variable;";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(returnRange), mutatedReturn);
              break;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("specializedFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}