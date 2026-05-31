//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Function_146
 */ 
class MutatorFrontendAction_146 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(146)

private:
    class MutatorASTConsumer_146 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_146(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_function_146.h"

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionName = FD->getNameAsString();
            auto returnType = FD->getReturnType().getAsString();
            std::string templateFunction = "template<int N>\n" + returnType + " " + functionName + "_template() {\n";
            templateFunction += "if constexpr (N > 0) {\nreturn " + functionName + "_template<N-1>();\n} else {\nreturn " + functionName + "();\n}\n}\n";

            Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut146*/" + templateFunction);
        }
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition(), unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}