//header file
#pragma once
#include "Mutator_base.h"

/**
 * BracedInitializerMacroArgMutation_317
 */ 
class MutatorFrontendAction_BracedInitializerMacroArgMutation_317 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(BracedInitializerMacroArgMutation_317)
private:
    class MutatorASTConsumer_BracedInitializerMacroArgMutation_317 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_BracedInitializerMacroArgMutation_317(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<std::string> availableStructs;
    };
};

//source file
#include "../include/Mutator_BracedInitializerMacroArgMutation_317.h"

// ========================================================================================================
#define MUTBracedInitializerMacroArgMutation_317_OUTPUT 1

void MutatorFrontendAction_BracedInitializerMacroArgMutation_317::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        availableStructs.push_back(RD->getNameAsString());
    } else if (auto *MC = Result.Nodes.getNodeAs<clang::MacroCall>("macroCall")) {
        if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(MC->getLocation()))
            return;

        const clang::ArrayRef<clang::Expr*> Args = MC->getArgs();
        if (Args.empty())
            return;

        for (unsigned i = 0; i < Args.size(); ++i) {
            const clang::Expr *Arg = Args[i];
            if (!Arg)
                continue;

            SourceRange ArgRange = Arg->getSourceRange();
            if (ArgRange.isInvalid())
                continue;

            std::string ArgText = stringutils::rangetoStr(*Result.SourceManager, ArgRange);
            if (ArgText.front() == '{' && ArgText.back() == '}') {
                if (!availableStructs.empty()) {
                    std::string StructName = availableStructs[getrandom::getRandomIndex(availableStructs.size())];
                    std::string NewArg = StructName + "{1, 2}";
                    Rewrite.ReplaceText(ArgRange, NewArg);
                }
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_BracedInitializerMacroArgMutation_317::MutatorASTConsumer_BracedInitializerMacroArgMutation_317::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::macroCall().bind("macroCall") || clang::ast_matchers::cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}