//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateSpecializationOrderMutator_70
 */ 
class MutatorFrontendAction_70 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_70>(TheRewriter);
    }

private:
    Rewriter TheRewriter;
    class MutatorASTConsumer_70 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_70(Rewriter &R) : rewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &rewriter;
    };

    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::ClassTemplateSpecializationDecl*> specializations;
    };
};

//source file
#include "../include/TemplateSpecializationOrderMutator_70.h"

// ========================================================================================================
#define MUT70_OUTPUT 1

void MutatorFrontendAction_70::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        auto specializations = TD->getSpecializations();
        for (auto specialization : specializations) {
            this->specializations.push_back(specialization);
        }
        if (this->specializations.size() > 1) {
            std::vector<std::vector<const clang::ClassTemplateSpecializationDecl*>> permutations;
            std::function<void(std::vector<const clang::ClassTemplateSpecializationDecl*>, size_t)> generatePermutations =
                [&](std::vector<const clang::ClassTemplateSpecializationDecl*> current, size_t start) {
                    if (start == current.size()) {
                        permutations.push_back(current);
                    } else {
                        for (size_t i = start; i < current.size(); i++) {
                            std::swap(current[start], current[i]);
                            generatePermutations(current, start + 1);
                            std::swap(current[start], current[i]);
                        }
                    }
                };
            generatePermutations(this->specializations, 0);
            for (auto permutation : permutations) {
                std::string mutatedCode;
                for (auto specialization : permutation) {
                    mutatedCode += stringutils::rangetoStr(*(Result.SourceManager), specialization->getSourceRange());
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), mutatedCode);
            }
        }
    }
}

void MutatorFrontendAction_70::MutatorASTConsumer_70::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(rewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}