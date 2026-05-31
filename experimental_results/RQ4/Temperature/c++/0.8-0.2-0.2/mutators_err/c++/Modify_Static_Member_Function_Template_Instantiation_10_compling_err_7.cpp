//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_static_member_function_template_instantiation_10
 */ 
class MutatorFrontendAction_10 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(10)

private:
    class MutatorASTConsumer_10 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_10(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_static_member_function_template_instantiation_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("StaticMemberFunction")) {
        if (!MT || !Result.SourceManager->isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isStatic() && MT->getTemplateSpecializationKind() == TSK_ImplicitInstantiation) {
            std::string funcDecl = Lexer::getSourceText(CharSourceRange::getTokenRange(MT->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts());

            // Introduce an incorrect template parameter
            size_t templatePos = funcDecl.find('<');
            if (templatePos != std::string::npos) {
                funcDecl.insert(templatePos + 1, "int, ");
            } else {
                funcDecl.insert(funcDecl.find('('), "<int>");
            }

            Rewrite.ReplaceText(MT->getSourceRange(), funcDecl);
        }
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isStaticStorageClass(), isTemplateInstantiation()).bind("StaticMemberFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}