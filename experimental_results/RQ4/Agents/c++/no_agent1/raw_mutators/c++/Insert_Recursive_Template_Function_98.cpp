//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Function_98
 */ 
class MutatorFrontendAction_98 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(98)

private:
    class MutatorASTConsumer_98 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_98(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_function_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionName = FD->getNameAsString();
            auto returnType = FD->getReturnType().getAsString();
            auto templateFunction = "template<int N>\n" + returnType + " " + functionName + "_recursive() {\n"
                                    "if constexpr (N > 0) return " + functionName + "_recursive<N-1>();\n"
                                    "else return " + functionName + "();\n}\n";

            auto functionSourceRange = FD->getSourceRange();
            Rewrite.InsertTextBefore(functionSourceRange.getBegin(), "/*mut98*/" + templateFunction);
        }
    }
}

void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}