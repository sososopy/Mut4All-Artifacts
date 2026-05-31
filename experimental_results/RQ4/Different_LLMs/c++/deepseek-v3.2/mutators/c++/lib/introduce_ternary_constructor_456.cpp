//source file
#include "../include/Introduce_Ternary_Constructor_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 1

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      classTemplates.push_back(CT);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::RecordDecl>("ForwardDeclaredClass")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      if (!FD->isCompleteDefinition()) {
        forwardDeclaredClasses.push_back(FD);
      }
    }
    else if (auto *ST = Result.Nodes.getNodeAs<clang::Stmt>("TargetStmt")) {
      //Filter nodes in header files
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getBeginLoc()))
        return;
      //Check if we have collected necessary types
      if (classTemplates.empty() || forwardDeclaredClasses.empty()) {
        return;
      }
      //Get the source code text of target node
      auto stmtText = stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t templateIdx = getrandom::getRandomIndex(classTemplates.size() - 1);
      size_t forwardIdx = getrandom::getRandomIndex(forwardDeclaredClasses.size() - 1);
      const clang::ClassTemplateDecl* selectedTemplate = classTemplates[templateIdx];
      const clang::RecordDecl* selectedForward = forwardDeclaredClasses[forwardIdx];
      std::string templateName = selectedTemplate->getNameAsString();
      std::string forwardName = selectedForward->getNameAsString();
      std::string mutatedStmt = "0 ? " + templateName + "<" + forwardName + ">{} : " + templateName + "<" + forwardName + ">{};";
      mutatedStmt = "/*mut456*/" + mutatedStmt;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), mutatedStmt);
    }
}
  
void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher forwardMatcher = recordDecl().bind("ForwardDeclaredClass");
    StatementMatcher stmtMatcher = stmt().bind("TargetStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(forwardMatcher, &callback);
    matchFinder.addMatcher(stmtMatcher, &callback);
    matchFinder.matchAST(Context);
}