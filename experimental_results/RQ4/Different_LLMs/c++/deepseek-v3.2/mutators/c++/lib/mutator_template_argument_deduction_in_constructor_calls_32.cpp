//source file
#include "../include/Mutator_Template_Argument_Deduction_In_Constructor_Calls_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("constructExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Check if constructor belongs to a templated class
      const clang::CXXRecordDecl *recordDecl = CE->getConstructor()->getParent();
      if (!recordDecl || !recordDecl->getDescribedClassTemplate())
        return;
      //Check if at least one argument is a callable type
      bool hasCallableArg = false;
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        clang::QualType argType = CE->getArg(i)->getType();
        if (argType->isFunctionPointerType() || argType->isMemberFunctionPointerType() ||
            argType->isBlockPointerType() || argType->isFunctionType() ||
            argType->isRecordType()) {
          const clang::RecordType *recordType = argType->getAs<clang::RecordType>();
          if (recordType) {
            const clang::CXXRecordDecl *argRecord = clang::dyn_cast<clang::CXXRecordDecl>(recordType->getDecl());
            if (argRecord && argRecord->isLambda())
              hasCallableArg = true;
          }
          if (argType->isFunctionPointerType() || argType->isMemberFunctionPointerType() ||
              argType->isBlockPointerType() || argType->isFunctionType())
            hasCallableArg = true;
        }
        if (hasCallableArg) break;
      }
      if (!hasCallableArg)
        return;
      //Record the node information to be used in the mutation process
      targetConstructs.push_back(CE);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("constructExprForMutation")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Ensure this is one of the recorded constructs
      if (std::find(targetConstructs.begin(), targetConstructs.end(), CE) == targetConstructs.end())
        return;
      //Get the source code text of target node
      clang::SourceRange range = CE->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      //Find a callable argument
      int callableArgIndex = -1;
      const clang::Expr *callableExpr = nullptr;
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        clang::QualType argType = CE->getArg(i)->getType();
        if (argType->isFunctionPointerType() || argType->isMemberFunctionPointerType() ||
            argType->isBlockPointerType() || argType->isFunctionType() ||
            argType->isRecordType()) {
          const clang::RecordType *recordType = argType->getAs<clang::RecordType>();
          if (recordType) {
            const clang::CXXRecordDecl *argRecord = clang::dyn_cast<clang::CXXRecordDecl>(recordType->getDecl());
            if (argRecord && argRecord->isLambda()) {
              callableArgIndex = i;
              callableExpr = CE->getArg(i);
              break;
            }
          }
          if (argType->isFunctionPointerType() || argType->isMemberFunctionPointerType() ||
              argType->isBlockPointerType() || argType->isFunctionType()) {
            callableArgIndex = i;
            callableExpr = CE->getArg(i);
            break;
          }
        }
      }
      if (callableArgIndex == -1 || !callableExpr)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        //Introduce temporary variable
        std::string tempVarName = "temp_callable_" + std::to_string(getrandom::getRandomIndex(1000));
        std::string callableExprText = stringutils::rangetoStr(*(Result.SourceManager), callableExpr->getSourceRange());
        mutatedText = "auto " + tempVarName + " = " + callableExprText + ";\n" + originalText;
        //Replace the callable argument with the temporary variable
        size_t pos = mutatedText.find(callableExprText, mutatedText.find(";\n") + 2);
        if (pos != std::string::npos) {
          mutatedText.replace(pos, callableExprText.length(), tempVarName);
        }
      } else {
        //Use static_cast
        std::string callableExprText = stringutils::rangetoStr(*(Result.SourceManager), callableExpr->getSourceRange());
        mutatedText = originalText;
        size_t pos = mutatedText.find(callableExprText);
        if (pos != std::string::npos) {
          std::string castExpr = "static_cast<decltype(" + callableExprText + ")>(" + callableExprText + ")";
          mutatedText.replace(pos, callableExprText.length(), castExpr);
        }
      }
      mutatedText = "/*mut32*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr(isExpansionInMainFile()).bind("constructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    //Second pass to perform mutation on recorded constructs
    MatchFinder mutationFinder;
    StatementMatcher mutationMatcher = cxxConstructExpr(isExpansionInMainFile()).bind("constructExprForMutation");
    mutationFinder.addMatcher(mutationMatcher, &callback);
    mutationFinder.matchAST(Context);
}