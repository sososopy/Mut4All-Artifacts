//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameter_Pack_Expansion_86
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
#include "../include/Modify_Template_Parameter_Pack_Expansion_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *Spec = dyn_cast<ClassTemplateSpecializationDecl>(TD->getTemplatedDecl())) {
            auto SourceRange = Spec->getSourceRange();
            auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);

            std::string MutatedText = OriginalText;
            size_t Pos = MutatedText.find("struct bar");
            if (Pos != std::string::npos) {
                MutatedText.insert(Pos + 10, ", size_t... N");
                size_t FuncPos = MutatedText.find("auto func");
                if (FuncPos != std::string::npos) {
                    size_t ParamPos = MutatedText.find(")", FuncPos);
                    if (ParamPos != std::string::npos) {
                        MutatedText.insert(ParamPos, ", int (&...arr)[N]");
                    }
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
        }
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        hasDescendant(cxxRecordDecl(isDerivedFrom(hasType(cxxRecordDecl()))))
    ).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}