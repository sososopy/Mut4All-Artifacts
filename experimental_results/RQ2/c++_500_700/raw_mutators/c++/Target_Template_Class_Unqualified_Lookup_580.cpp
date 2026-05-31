//header file
#pragma once
#include "Mutator_base.h"

/**
 * target_template_class_unqualified_lookup_580
 */ 
class MutatorFrontendAction_580 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(580)

private:
    class MutatorASTConsumer_580 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_580(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

//source file
#include "../include/target_template_class_unqualified_lookup_580.h"

// ========================================================================================================
#define MUT580_OUTPUT 1

void MutatorFrontendAction_580::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getTemplatedDecl()->isCompleteDefinition()) {
            templateClasses.push_back(TD->getTemplatedDecl());
        }
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;

        if (Derived->isCompleteDefinition()) {
            for (const auto *TemplateClass : templateClasses) {
                if (Derived->isDerivedFrom(TemplateClass)) {
                    std::string baseName = TemplateClass->getNameAsString();
                    std::string mutation = "public Base, ";
                    std::string newBase = mutation + baseName;
                    std::string derivedText = stringutils::rangetoStr(*(Result.SourceManager), Derived->getSourceRange());
                    size_t pos = derivedText.find(baseName);
                    if (pos != std::string::npos) {
                        derivedText.replace(pos, baseName.length(), newBase);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Derived->getSourceRange()), derivedText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_580::MutatorASTConsumer_580::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}