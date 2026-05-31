//header file
#pragma once
#include "Mutator_base.h"

/**
 * Reorder_Base_Class_Initializers_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)
private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Reorder_Base_Class_Initializers_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctor")) {
        if (!ctor || !Result.Context->getSourceManager().isWrittenInMainFile(ctor->getLocation()))
            return;
        
        SmallVector<clang::CXXCtorInitializer*, 4> baseInits;
        for (auto *init : ctor->inits()) {
            if (init && init->isBaseInitializer()) {
                baseInits.push_back(init);
            }
        }
        if (baseInits.size() < 2) return;
        
        std::vector<std::string> initTexts;
        for (auto *init : baseInits) {
            std::string text = stringutils::rangetoStr(*Result.SourceManager, init->getSourceRange());
            size_t pos = text.find_first_not_of(" ,\t");
            if (pos != std::string::npos) {
                text = text.substr(pos);
            }
            initTexts.push_back(text);
        }
        
        std::reverse(initTexts.begin(), initTexts.end());
        std::string newInitList = ": ";
        for (size_t i = 0; i < initTexts.size(); ++i) {
            if (i > 0) newInitList += ", ";
            newInitList += initTexts[i];
        }
        
        SourceLocation start = baseInits[0]->getSourceRange().getBegin();
        SourceLocation end = baseInits.back()->getSourceRange().getEnd();
        Rewrite.ReplaceText(SourceRange(start, end), newInitList);
    }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(has(cxxConstructorInitializer(isBaseInitializer()))).bind("ctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}