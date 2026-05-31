//header file
#pragma once
#include "Mutator_base.h"

/**
 * ConceptWithTemplateTemplateInConstrainedAuto_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ConceptWithTemplateTemplateInConstrainedAuto_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcDecl")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        FunctionDecl *FD = FTD->getTemplatedDecl();
        std::string conceptCode = R"(
template <typename T, template <class> class Op>
concept InvokableCheck_103 = std::is_invocable_v<Op<void>, decltype(777), T>;
        )";
        SourceLocation funcLoc = FD->getBeginLoc();
        Rewrite.InsertTextBefore(funcLoc, conceptCode);
        for (auto *param : FD->parameters()) {
            if (auto *TTP = param->getType()->getAs<TemplateTypeParmType>()) {
                SourceRange typeRange = param->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                std::string newType = "InvokableCheck_103<std::plus> auto";
                Rewrite.ReplaceText(typeRange, newType);
            }
        }
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasAnyParameter(hasType(templateTypeParmType()))).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}