//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Binary_Operator_With_Compound_Statement_220
 */ 
class MutatorFrontendAction_220 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(220)

private:
    class MutatorASTConsumer_220 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_220(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::Type*> integralTypes;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getLocation()))
        return;
      
      // Check if we are in a constexpr function context
      const clang::FunctionDecl* FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstExprFunc");
      if (!FD || !FD->isConstexpr())
        return;
      
      // Check if binary operator is in a conditional statement (if, while, for, etc.)
      const clang::Stmt* Parent = Result.Nodes.getNodeAs<clang::Stmt>("ConditionalParent");
      if (!Parent)
        return;
      
      // Check if operands are integral or enumeration types
      clang::Expr* LHS = BO->getLHS();
      clang::Expr* RHS = BO->getRHS();
      if (!LHS || !RHS)
        return;
      
      clang::QualType LHSType = LHS->getType();
      clang::QualType RHSType = RHS->getType();
      if (!LHSType->isIntegralOrEnumerationType() || !RHSType->isIntegralOrEnumerationType())
        return;
      
      // Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), BO->getSourceRange());
      
      // Determine integral type to use for local variable
      std::string typeName = "int";
      if (!integralTypes.empty()) {
        // Try to find a type from the seed program
        for (const clang::Type* type : integralTypes) {
          if (type->isIntegralOrEnumerationType()) {
            typeName = type->getAsCXXRecordDecl() ? type->getAsCXXRecordDecl()->getNameAsString() : "int";
            break;
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "({ " + typeName + " local_var; local_var = 0; if (local_var) break; })";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedText);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("IntegralType")) {
      // Collect integral types from the seed program
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      clang::QualType QT = TD->getTypeForDecl()->getCanonicalTypeInternal();
      if (QT->isIntegralOrEnumerationType()) {
        integralTypes.push_back(QT.getTypePtr());
      }
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher to collect integral types
    DeclarationMatcher typeMatcher = typeDecl().bind("IntegralType");
    
    // Second matcher to find binary operators in constexpr functions' conditional statements
    StatementMatcher binaryOpMatcher = 
        binaryOperator(
            hasParent(
                stmt(anyOf(
                    ifStmt(),
                    whileStmt(),
                    forStmt(),
                    doStmt()
                )).bind("ConditionalParent")
            ),
            inside(
                functionDecl(isConstexpr()).bind("ConstExprFunc")
            )
        ).bind("BinaryOp");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(binaryOpMatcher, &callback);
    matchFinder.matchAST(Context);
}