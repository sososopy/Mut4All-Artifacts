//source file
#include "../include/Mutator_Replace_Fold_Expression_In_Nested_Lambda_132.h"

// ========================================================================================================
#define MUT132_OUTPUT 1

void MutatorFrontendAction_132::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      // Check if this lambda is nested inside another lambda
      auto parent = Result.Context->getParents(*LE);
      bool isNested = false;
      while (!parent.empty()) {
        if (parent[0].get<clang::LambdaExpr>()) {
          isNested = true;
          break;
        }
        parent = Result.Context->getParents(parent[0]);
      }
      
      if (!isNested) return;
      
      // Check if lambda is inside a template
      bool inTemplate = false;
      auto currentDecl = LE->getLambdaClass();
      while (currentDecl) {
        if (currentDecl->getDescribedClassTemplate() || 
            currentDecl->getInstantiatedFromMemberClass()) {
          inTemplate = true;
          break;
        }
        currentDecl = dyn_cast<clang::CXXRecordDecl>(currentDecl->getParent());
      }
      
      if (!inTemplate) return;
      
      // Look for fold expressions in the lambda body
      auto body = LE->getBody();
      if (!body) return;
      
      clang::BinaryOperator* foldExpr = nullptr;
      clang::Stmt* stmtWithFold = nullptr;
      
      std::function<void(clang::Stmt*)> findFoldExpr = [&](clang::Stmt* S) {
        if (auto* BO = dyn_cast<clang::BinaryOperator>(S)) {
          if (BO->getOpcode() == clang::BO_PtrMemD || BO->getOpcode() == clang::BO_PtrMemI) {
            foldExpr = BO;
            stmtWithFold = S;
            return;
          }
        }
        for (auto child : S->children()) {
          if (child) findFoldExpr(child);
        }
      };
      
      findFoldExpr(body);
      
      if (!foldExpr) return;
      
      // Get the source code text of target node
      std::string foldText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                    foldExpr->getSourceRange());
      
      // Generate replacement by expanding the parameter pack
      std::string replacement;
      auto lhs = foldExpr->getLHS();
      auto rhs = foldExpr->getRHS();
      
      // Get pattern text
      std::string lhsText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    lhs->getSourceRange());
      std::string rhsText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    rhs->getSourceRange());
      
      // Create helper template for expansion
      std::string helperTemplate = R"(
template<typename... Ts>
struct ExpandHelper_132 {
  static void expand() {
    []() { )";
    
      // Remove the fold operator and pack from pattern
      size_t packPos = lhsText.find("...");
      if (packPos != std::string::npos) {
        lhsText.erase(packPos, 3);
      }
      packPos = rhsText.find("...");
      if (packPos != std::string::npos) {
        rhsText.erase(packPos, 3);
      }
      
      // Create expanded version with commas
      replacement = "(";
      replacement += lhsText + ", " + rhsText + ", " + lhsText + ")";
      
      // Store for replacement
      foldExpressions[LE] = replacement;
      
    } else if (auto *FE = Result.Nodes.getNodeAs<clang::BinaryOperator>("foldExpr")) {
      // Filter nodes in header files
      if (!FE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FE->getBeginLoc()))
        return;
      
      if (FE->getOpcode() != clang::BO_PtrMemD && FE->getOpcode() != clang::BO_PtrMemI) return;
      
      // Find the containing lambda
      auto parents = Result.Context->getParents(*FE);
      const clang::LambdaExpr* containingLambda = nullptr;
      while (!parents.empty()) {
        if (auto* LE = parents[0].get<clang::LambdaExpr>()) {
          containingLambda = LE;
          break;
        }
        parents = Result.Context->getParents(parents[0]);
      }
      
      if (!containingLambda) return;
      
      // Check if we have a replacement for this lambda
      auto it = foldExpressions.find(containingLambda);
      if (it == foldExpressions.end()) return;
      
      //Get the source code text of target node
      std::string foldText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                    FE->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutated = "/*mut132*/" + it->second;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FE->getSourceRange()), mutated);
      
      // Clean up
      foldExpressions.erase(containingLambda);
    }
}
  
void MutatorFrontendAction_132::MutatorASTConsumer_132::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto lambdaMatcher = lambdaExpr().bind("lambdaExpr");
    auto foldMatcher = binaryOperator(hasOperatorName("...")).bind("foldExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(foldMatcher, &callback);
    matchFinder.matchAST(Context);
}