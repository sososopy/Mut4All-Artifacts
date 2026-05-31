//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Nested_Lambda_With_ParamPack_Fold_132
 */ 
class MutatorFrontendAction_Insert_Nested_Lambda_With_ParamPack_Fold_132 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(132)

private:
    class MutatorASTConsumer_Insert_Nested_Lambda_With_ParamPack_Fold_132 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Nested_Lambda_With_ParamPack_Fold_132(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Nested_Lambda_With_ParamPack_Fold_132.h"

// ========================================================================================================
#define MUT132_OUTPUT 1

void MutatorFrontendAction_Insert_Nested_Lambda_With_ParamPack_Fold_132::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody() || !FD->getTemplateParameters())
            return;
        auto *Body = FD->getBody();
        if (!Body)
            return;
        if (auto *CS = dyn_cast<clang::CompoundStmt>(Body)) {
            if (CS->getNumStmts() == 0)
                return;
            auto *FirstStmt = CS->getStmt(0);
            SourceRange range = FirstStmt->getSourceRange();
            std::string lambda;
            for (const auto *param : *FD->getTemplateParameters()) {
                if (const auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                    if (!lambda.empty())
                        lambda += ", ";
                    lambda += TTP->getNameAsString();
                }
            }
            lambda += ", int, float";
            std::string code = "[]<class... Us>() {\n    (static_cast<void>(Us{}), ...);\n}.template operator()<" + lambda + ">();\n";
            Rewrite.ReplaceText(range, code);
        }
    }
}
  
void MutatorFrontendAction_Insert_Nested_Lambda_With_ParamPack_Fold_132::MutatorASTConsumer_Insert_Nested_Lambda_With_ParamPack_Fold_132::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasBody(compoundStmt().bind("stmt"))).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}