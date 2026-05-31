//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Parameter_Pack_In_Lambda_Expression_126
 */ 
class MutatorFrontendAction_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(126)

private:
    class MutatorASTConsumer_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_126(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_parameter_pack_in_lambda_expression_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        //Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                         CE->getBeginLoc()))
            return;

        //Get the source code text of target node
        auto callExprText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    CE->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedExpr = "decltype([](auto... expanded) { return sizeof...(expanded); }(" + callExprText + "))";
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedExpr);
    }
}

void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasAncestor(decltypeType())).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}