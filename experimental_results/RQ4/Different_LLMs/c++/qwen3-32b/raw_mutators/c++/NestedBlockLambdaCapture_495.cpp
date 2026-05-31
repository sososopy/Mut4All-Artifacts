//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedBlockLambdaCapture_495
 */ 
class MutatorFrontendAction_495 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(495)

private:
    class MutatorASTConsumer_495 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_495(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedBlockLambdaCapture_495.h"

// ========================================================================================================
#define MUT495_OUTPUT 1

void MutatorFrontendAction_495::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getBeginLoc()))
            return;

        if (MT->captures().empty())
            return;

        std::string capturedVarName;
        for (const auto *capture : MT->captures()) {
            if (capture->isThisCapture())
                continue;
            const VarDecl *var = capture->getCapturedVar();
            if (var) {
                capturedVarName = var->getNameAsString();
                break;
            }
        }

        if (capturedVarName.empty())
            return;

        std::string blockCode = "^\n{\n    (void)" + capturedVarName + ";\n};\n";

        const CompoundStmt *body = MT->getBody();
        if (!body)
            return;

        SourceLocation bodyStart = body->getLBracLoc();
        Rewrite.InsertText(bodyStart, blockCode, true, true);
    }
}

void MutatorFrontendAction_495::MutatorASTConsumer_495::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasCapture(anything())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}