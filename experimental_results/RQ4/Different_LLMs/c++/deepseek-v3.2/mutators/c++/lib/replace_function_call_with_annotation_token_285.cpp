//source file
#include "../include/Mutator_Replace_Function_Call_With_Annotation_Token_285.h"

// ========================================================================================================
#define MUT285_OUTPUT 1

void MutatorFrontendAction_285::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("TargetCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto *FD = CE->getDirectCallee();
      if (!FD) return;
      
      // Check if return type is pointer or reference
      QualType retType = FD->getReturnType();
      if (!retType->isPointerType() && !retType->isReferenceType()) return;
      
      // Check if call is used as argument to another function/macro expecting identifier
      bool usedAsIdentifier = false;
      for (auto parent : Result.Context->getParents(*CE)) {
        if (parent.get<clang::CallExpr>()) {
          usedAsIdentifier = true;
          break;
        }
        if (parent.get<clang::Expr>()) {
          usedAsIdentifier = true;
          break;
        }
      }
      if (!usedAsIdentifier) return;
      
      // Record candidate call and its return type
      candidateCalls.push_back(CE);
      returnTypes.push_back(retType.getAsString());
      
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      // Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      
      // Check if this is a function returning pointer/reference (potential target for adding annotation function)
      QualType retType = FD->getReturnType();
      if (!retType->isPointerType() && !retType->isReferenceType()) return;
      
      // Add annotation function declaration before the original function
      std::string annotationFunc = "/*mut285*/" + retType.getAsString() + " getAnnotationToken() { return nullptr; }\n";
      Rewrite.InsertTextBefore(FD->getBeginLoc(), annotationFunc);
      
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("ReplaceCall")) {
      // Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Randomly select one candidate call to mutate
      if (candidateCalls.empty()) return;
      size_t idx = getrandom::getRandomIndex(candidateCalls.size() - 1);
      const clang::CallExpr* targetCall = candidateCalls[idx];
      std::string retTypeStr = returnTypes[idx];
      
      // Ensure this is the call we're processing
      if (CE != targetCall) return;
      
      // Get original function name
      auto *origFD = targetCall->getDirectCallee();
      if (!origFD) return;
      std::string origName = origFD->getNameAsString();
      
      // Perform mutation: replace original call with getAnnotationToken()
      std::string replacement = "getAnnotationToken()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(targetCall->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_285::MutatorASTConsumer_285::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto callMatcher = callExpr(hasAnyArgument(hasType(pointerType()))).bind("TargetCall");
    auto funcMatcher = functionDecl(hasAnyParameter(hasType(pointerType()))).bind("FunctionDecl");
    auto replaceMatcher = callExpr().bind("ReplaceCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(replaceMatcher, &callback);
    matchFinder.matchAST(Context);
}