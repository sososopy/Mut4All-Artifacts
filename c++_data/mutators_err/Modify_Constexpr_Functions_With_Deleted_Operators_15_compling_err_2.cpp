//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Constexpr_Functions_With_Deleted_Operators_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_constexpr_functions_with_deleted_operators_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConstexprMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Ensure the method is an operator overload and has a reference or pointer return type
      if (MT->isOverloadedOperator() && (MT->getReturnType()->isPointerType() || MT->getReturnType()->isReferenceType())) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        auto pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration.replace(pos, 1, " = delete;");
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}

void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isConstexpr(), isOverloadedOperator()).bind("ConstexprMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}