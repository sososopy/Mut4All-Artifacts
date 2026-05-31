//header file
#pragma once
#include "Mutator_base.h"

/**
 * Manipulate_Preprocessor_Directives_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(409)

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::set<std::string> visitedDirectives;
    };
};

//source file
#include "../include/Manipulate_Preprocessor_Directives_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("PreprocessorDirective")) {
        if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(PD->getLocation()))
            return;

        auto directiveText = stringutils::rangetoStr(*(Result.SourceManager), PD->getSourceRange());
        if (visitedDirectives.find(directiveText) != visitedDirectives.end())
            return;

        visitedDirectives.insert(directiveText);

        std::string mutatedDirective;
        if (directiveText.find("#ifdef") != std::string::npos || directiveText.find("#ifndef") != std::string::npos) {
            std::string macro = directiveText.substr(directiveText.find(' ')+1);
            mutatedDirective = "#if defined(" + macro + ") && !defined(UNUSED_MACRO)\n#define UNUSED_MACRO\n#undef " + macro + "\n";
        } else if (directiveText.find("#if") != std::string::npos) {
            mutatedDirective = "#if defined(UNUSED_MACRO) || !defined(ANOTHER_MACRO)\n#define ANOTHER_MACRO\n#undef UNUSED_MACRO\n";
        }

        if (!mutatedDirective.empty()) {
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()), mutatedDirective);
        }
    }
}
  
void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = preprocessingDirective().bind("PreprocessorDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}