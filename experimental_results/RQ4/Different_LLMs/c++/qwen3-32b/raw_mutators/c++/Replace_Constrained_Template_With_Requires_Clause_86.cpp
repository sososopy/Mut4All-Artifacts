//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constrained_Template_With_Requires_Clause_86
 */ 
class MutatorFrontendAction_86 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(86)

private:
    class MutatorASTConsumer_86 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_86(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constrained_Template_With_Requires_Clause_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;
        
        auto *TPList = FT->getTemplateParameters();
        if (!TPList || TPList->size() != 1)
            return;
        
        auto *TTD = TPList->getParam(0);
        if (!TTD || !TTD->isParameterPack() || !TTD->hasConcept())
            return;
        
        auto *CSD = TTD->getConcept();
        if (!CSD || !CSD->getConcept()->getName().equals("always_true"))
            return;
        
        auto TemplateArgs = CSD->getTemplateArgs();
        if (TemplateArgs.size() != 1)
            return;
        
        std::string TStr;
        llvm::raw_string_ostream OS(TStr);
        TemplateArgs.get(0).print(OS, *Result.Context);
        std::string T = OS.str();
        
        std::string U = TTD->getNameAsString();
        std::string requiresClause = " requires (always_true<" + T + ", " + U + "> && ...)";
        
        SourceLocation startLoc = TTD->getLocation();
        SourceLocation endLoc = TTD->getSourceRange().getEnd();
        
        std::string newParam = U + "...";
        
        std::string originalTemplate = "template <" + TTD->getSourceRange().getAsString(*Result.SourceManager) + ">";
        std::string newTemplate = "template <" + newParam + ">" + requiresClause;
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), newTemplate);
    }
}

void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasTemplateParameters(
            forEach(templateTypeParmDecl(
                isParameterPack(),
                hasConceptSpecialization(conceptDecl(hasName("always_true")))
            ))
        )
    ).bind("functionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}