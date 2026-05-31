//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Concept_Template_Template_Parameter_63
 */ 
class MutatorFrontendAction_63 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(63)

private:
    class MutatorASTConsumer_63 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_63(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> concepts;
    };
};

//source file
#include "../include/Mutator_Concept_Template_Template_Parameter_63.h"

// ========================================================================================================
#define MUT63_OUTPUT 1

void MutatorFrontendAction_63::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        concepts.push_back(CD->getNameAsString());
    } else if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateTemplateParmDecl>("templateParm")) {
        if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(TT->getLocation()))
            return;
        if (concepts.empty())
            return;
        SourceManager &SM = *Result.SourceManager;
        SourceLocation Start = TT->getBeginLoc();
        SourceLocation End = TT->getEndLoc();
        if (Start.isInvalid() || End.isInvalid())
            return;
        StringRef OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(Start, End), SM, Result.Context->getLangOpts());
        std::string ConceptName = concepts[0];
        std::string NewText = "template<" + ConceptName + "> typename " + TT->getNameAsString();
        Rewrite.ReplaceText(Start, OriginalText.size(), NewText);
    }
}
  
void MutatorFrontendAction_63::MutatorASTConsumer_63::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher conceptMatcher = conceptDecl().bind("concept");
    DeclarationMatcher templateParmMatcher = templateTemplateParmDecl(
        ofKind(ParmVarDecl::TemplateTemplateParm),
        unless(hasConcept()),
        hasAncestor(functionTemplateDecl()),
        hasNumTemplateParameters(1)
    ).bind("templateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(templateParmMatcher, &callback);
    matchFinder.matchAST(Context);
}