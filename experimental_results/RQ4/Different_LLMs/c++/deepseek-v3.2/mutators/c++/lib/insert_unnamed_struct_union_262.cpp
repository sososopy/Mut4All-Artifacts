//source file
#include "../include/Mutator_262.h"

// ========================================================================================================
#define MUT262_OUTPUT 1

void MutatorFrontendAction_262::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record candidate variable declarations for member names
      candidateVars.push_back(VD);
      //Record candidate types for member types
      candidateTypes.push_back(VD->getType().getTypePtr());
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      //Get the source code text of target node
      auto bodyRange = FD->getBody()->getSourceRange();
      SourceLocation insertLoc = bodyRange.getBegin().getLocWithOffset(1);
      //Perform mutation on the source code text by applying string replacement
      std::string mutationText;
      int dice = getrandom::getRandomIndex(2);
      if (dice == 0) {
        // struct
        mutationText = "struct { ";
      } else {
        // union
        mutationText = "union { ";
      }
      
      // Choose member type and name
      std::string memberType;
      std::string memberName;
      if (candidateVars.empty()) {
        memberType = "int";
        memberName = "data";
      } else {
        size_t idx = getrandom::getRandomIndex(candidateVars.size() - 1);
        const clang::VarDecl* chosenVar = candidateVars[idx];
        memberType = chosenVar->getType().getAsString();
        memberName = chosenVar->getNameAsString();
      }
      
      mutationText += memberType + " " + memberName + "; };";
      mutationText = "\n/*mut262*/" + mutationText;
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insertLoc, mutationText);
    }
}
  
void MutatorFrontendAction_262::MutatorASTConsumer_262::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}