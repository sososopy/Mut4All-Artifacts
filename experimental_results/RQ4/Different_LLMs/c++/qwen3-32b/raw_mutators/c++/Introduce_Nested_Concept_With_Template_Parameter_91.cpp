//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Nested_Concept_With_Template_Parameter_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Nested_Concept_With_Template_Parameter_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("concept")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation conceptLoc = MT->getLocation();

        // Insert nested concept D
        std::string dConcept = R"(
template <template <typename> class T>
concept D = true;
)";
        Rewrite.InsertTextBefore(conceptLoc, dConcept);

        // Modify requires clause
        auto *RE = MT->getConstraintExpr();
        if (!RE || !isa<clang::RequiresExpr>(RE))
            return;

        SourceRange reRange = RE->getSourceRange();
        std::string reText = stringutils::rangetoStr(SM, reRange);
        size_t closeBracePos = reText.find_last_of('}');
        if (closeBracePos == std::string::npos)
            return;

        std::string newRequires = "  requires D<decltype(a)>;\n";
        std::string modifiedText = reText.substr(0, closeBracePos) + newRequires + reText.substr(closeBracePos);
        Rewrite.ReplaceText(reRange, modifiedText);
    }
}
  
void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}