//source file
#include "../include/Mutator_Introduce_Lambda_Decltype_Variadic_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record candidate functions with local variables
      if (FD->hasBody()) {
        candidateFunctions.push_back(FD);
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record candidate local variables
      if (VD->isLocalVarDecl() && VD->hasLocalStorage()) {
        candidateVariables.push_back(VD);
      }
    }
    else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplateDecl")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      //Record candidate template functions
      candidateTemplates.push_back(FTD);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TargetFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasBody()) return;
      string funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      
      //Find a local variable in this function
      const clang::VarDecl* targetVar = nullptr;
      for (const clang::VarDecl* vd : candidateVariables) {
        if (vd->getDeclContext() == FD) {
          targetVar = vd;
          break;
        }
      }
      if (!targetVar) return;
      
      //Find a template function to adapt
      const clang::FunctionTemplateDecl* targetTemplate = nullptr;
      if (!candidateTemplates.empty()) {
        size_t idx = getrandom::getRandomIndex(candidateTemplates.size() - 1);
        targetTemplate = candidateTemplates[idx];
      }
      
      //Perform mutation on the source code text by applying string replacement
      string varName = targetVar->getNameAsString();
      string varType = targetVar->getType().getAsString();
      
      //Insert lambda inside function body
      string lambdaInsertion;
      if (getrandom::getRandomIndex(1) == 0) {
        lambdaInsertion = "auto lam = [](auto param) -> decltype(" + varName + ") { return " + varName + "; };";
      } else {
        lambdaInsertion = "auto lam = [&" + varName + "](auto param) -> decltype(" + varName + ") { return " + varName + "; };";
      }
      
      //Find position after last local variable declaration in body
      SourceLocation insertLoc = targetVar->getEndLoc();
      insertLoc = Rewrite.getSourceMgr().getLocForEndOfFile(Rewrite.getSourceMgr().getFileID(insertLoc));
      
      //Create variadic template expansion
      string templateInsertion;
      if (targetTemplate) {
        string templateName = targetTemplate->getNameAsString();
        templateInsertion = "\n" + templateName + "<int, double>(lam);";
      } else {
        //Create new variadic template function
        templateInsertion = "\ntemplate<typename... Ts> void bar(auto f) { ((Ts{}, f(0)), ...); }\nbar<int, double>(lam);";
      }
      
      //Combine mutations
      string mutation = "\n/*mut440*/" + lambdaInsertion + templateInsertion;
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, mutation);
    }
}
  
void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("FunctionTemplateDecl");
    DeclarationMatcher targetMatcher = functionDecl(hasBody(compoundStmt())).bind("TargetFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.matchAST(Context);
}