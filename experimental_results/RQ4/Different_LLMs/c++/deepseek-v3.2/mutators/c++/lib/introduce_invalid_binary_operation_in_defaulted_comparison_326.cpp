//source file
#include "../include/Mutator_Introduce_Invalid_Binary_Operation_In_Defaulted_Comparison_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("DefaultedComparison")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (FD->isDefaulted() && FD->getNumParams() == 2) {
        auto paramTypes = FD->getParamDecl(0)->getType();
        if (paramTypes->isRecordType()) {
          const clang::CXXRecordDecl *RD = paramTypes->getAsCXXRecordDecl();
          if (RD) {
            classesWithDefaultedComparison.push_back(RD);
          }
        }
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ComparisonTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (FD->isTemplateInstantiation() || FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate) {
        if (FD->getNumParams() == 2 && FD->getNameAsString().find("operator") != std::string::npos) {
          candidateComparisonTemplates.push_back(FD);
        }
      }
    }
    else if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOpInTemplate")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      //Get the source code text of target node
      if (BO->isComparisonOp()) {
        auto *LHS = BO->getLHS()->IgnoreImpCasts();
        auto *RHS = BO->getRHS()->IgnoreImpCasts();
        //Check if the binary operator is inside a candidate comparison template
        auto Parents = Result.Context->getParents(*BO);
        if (!Parents.empty()) {
          const clang::FunctionDecl *ParentFD = Parents[0].get<clang::FunctionDecl>();
          if (ParentFD && std::find(candidateComparisonTemplates.begin(), candidateComparisonTemplates.end(), ParentFD) != candidateComparisonTemplates.end()) {
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedExpr = "0 == ";
            mutatedExpr += stringutils::rangetoStr(*(Result.SourceManager), RHS->getSourceRange());
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedExpr);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher defaultedComparisonMatcher = functionDecl(isDefaulted(), hasName("operator==")).bind("DefaultedComparison");
    DeclarationMatcher comparisonTemplateMatcher = functionDecl(isTemplateInstantiation(), hasName("operator==")).bind("ComparisonTemplate");
    StatementMatcher binaryOpMatcher = binaryOperator(isComparisonOperator()).bind("BinaryOpInTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(defaultedComparisonMatcher, &callback);
    matchFinder.addMatcher(comparisonTemplateMatcher, &callback);
    matchFinder.addMatcher(binaryOpMatcher, &callback);
    matchFinder.matchAST(Context);
}