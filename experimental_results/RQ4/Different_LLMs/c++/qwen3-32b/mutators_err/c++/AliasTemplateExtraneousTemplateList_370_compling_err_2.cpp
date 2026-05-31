//header file
#pragma once
#include "Mutator_base.h"

/**
 * AliasTemplateExtraneousTemplateList_370
 */ 
class MutatorFrontendAction_370 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(370)

private:
    class MutatorASTConsumer_370 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_370(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/AliasTemplateExtraneousTemplateList_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("aliasTemplate")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        SourceManager &SM = Result.Context->getSourceManager();
        CharSourceRange range = CharSourceRange::getTokenRange(TD->getSourceRange());
        std::string original = stringutils::rangetoStr(SM, range.getAsRange());
        
        size_t using_pos = original.find("using");
        if (using_pos == std::string::npos) return;
        
        std::string new_template = "template<typename U>";
        std::string mutated = original.substr(0, using_pos) + new_template + original.substr(using_pos);
        
        Rewrite.ReplaceText(range, mutated);
    }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = aliasTemplateDecl(has(usingDecl())).bind("aliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}