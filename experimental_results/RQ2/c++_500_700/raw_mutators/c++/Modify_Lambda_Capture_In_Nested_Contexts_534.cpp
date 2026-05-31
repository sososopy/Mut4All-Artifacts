//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_lambda_capture_in_nested_contexts_534
 */ 
class MutatorFrontendAction_534 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(534)

private:
    class MutatorASTConsumer_534 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_534(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> outerVariables;
    };
};

//source file
#include "../include/Modify_Lambda_Capture_In_Nested_Contexts_534.h"

// ========================================================================================================
#define MUT534_OUTPUT 1

void MutatorFrontendAction_534::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *InnerLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("InnerLambda")) {
        if (!InnerLambda || !Result.Context->getSourceManager().isWrittenInMainFile(
                InnerLambda->getBeginLoc()))
            return;

        if (auto *OuterLambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("OuterLambda")) {
            if (!OuterLambda)
                return;

            if (outerVariables.empty()) {
                for (auto *Decl : OuterLambda->getLambdaClass()->decls()) {
                    if (auto *VD = llvm::dyn_cast<clang::VarDecl>(Decl)) {
                        outerVariables.push_back(VD);
                    }
                }
            }

            if (!outerVariables.empty()) {
                auto *CaptureList = InnerLambda->getCaptureList();
                std::string newCapture = "";
                for (auto *Var : outerVariables) {
                    if (!std::any_of(CaptureList.begin(), CaptureList.end(),
                                     [Var](const clang::LambdaCapture &Capture) {
                                         return Capture.capturesVariable() && Capture.getCapturedVar() == Var;
                                     })) {
                        newCapture = Var->getNameAsString();
                        break;
                    }
                }

                if (!newCapture.empty()) {
                    auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                                InnerLambda->getSourceRange());
                    size_t bracketPos = lambdaSource.find(']');
                    if (bracketPos != std::string::npos) {
                        lambdaSource.insert(bracketPos, ", " + newCapture);
                        size_t bodyPos = lambdaSource.find('{', bracketPos);
                        if (bodyPos != std::string::npos) {
                            lambdaSource.insert(bodyPos + 1, newCapture + "; ");
                        }
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InnerLambda->getSourceRange()), lambdaSource);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_534::MutatorASTConsumer_534::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto innerLambdaMatcher = lambdaExpr(hasAncestor(lambdaExpr().bind("OuterLambda"))).bind("InnerLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(innerLambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}