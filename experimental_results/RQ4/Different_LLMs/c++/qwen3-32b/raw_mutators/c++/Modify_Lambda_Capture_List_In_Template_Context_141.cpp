//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_List_In_Template_Context_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Capture_List_In_Template_Context_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        const FunctionDecl *parentFunc = nullptr;
        const DeclContext *dc = lambda->getDeclContext();
        if (const auto *fd = dyn_cast<FunctionDecl>(dc)) {
            parentFunc = fd;
        } else {
            return;
        }

        std::vector<const VarDecl*> vars;
        for (const auto *d : parentFunc->decls()) {
            if (const auto *vd = dyn_cast<VarDecl>(d)) {
                vars.push_back(vd);
            }
        }

        for (const auto *vd : vars) {
            bool captured = false;
            for (const auto *capture : lambda->capture_list()) {
                if (capture->getCapturedVar() == vd) {
                    captured = true;
                    break;
                }
            }
            if (!captured) {
                std::string varName = vd->getNameAsString();
                std::string newCapture = "[" + varName + "]";
                SourceLocation lParenLoc = lambda->getLParenLoc();
                SourceLocation rParenLoc = lambda->getRParenLoc();
                Rewrite.ReplaceText(SourceRange(lParenLoc, rParenLoc), newCapture);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        anyOf(
            hasAncestor(functionDecl(isTemplateInstantiation())),
            hasAncestor(classTemplateSpecializationDecl())
        )
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}