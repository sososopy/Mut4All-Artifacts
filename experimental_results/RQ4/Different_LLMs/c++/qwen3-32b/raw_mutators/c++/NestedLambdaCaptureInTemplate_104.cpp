//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedLambdaCaptureInTemplate_104
 */ 
class MutatorFrontendAction_104 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(104)

private:
    class MutatorASTConsumer_104 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_104(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedLambdaCaptureInTemplate_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(lambda->getLocation()))
            return;

        std::string lambdaCode = stringutils::rangetoStr(*Result.SourceManager, lambda->getSourceRange());
        size_t openBracket = lambdaCode.find('[');
        size_t closeBracket = lambdaCode.find(']');
        if (openBracket != std::string::npos && closeBracket != std::string::npos && openBracket + 1 == closeBracket) {
            auto *parentFunc = lambda->getFunctionParent();
            std::string varToCapture;
            for (const auto *decl : parentFunc->decls()) {
                if (auto *var = dyn_cast<VarDecl>(decl)) {
                    varToCapture = var->getNameAsString();
                    break;
                }
            }

            if (!varToCapture.empty()) {
                std::string newCapture = "[" + varToCapture + "]";
                lambdaCode.replace(openBracket, closeBracket - openBracket, newCapture);
                Rewrite.ReplaceText(lambda->getSourceRange(), lambdaCode);
            }
        }
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(
        hasAncestor(functionDecl(hasParent(functionTemplateDecl())))
    ).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}