```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_class_specialization_with_constraints_532
 */ 
class MutatorFrontendAction_532 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(532)

private:
    class MutatorASTConsumer_532 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_532(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_class_specialization_with_constraints_532.h"

// ========================================================================================================
#define MUT532_OUTPUT 1

void MutatorFrontendAction_532::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        for (auto *spec : CTD->specializations()) {
            if (spec->isExplicitSpecialization())
                continue;

            auto templateName = CTD->getNameAsString();
            auto specSource = stringutils::rangetoStr(*(Result.SourceManager), spec->getSourceRange());

            std::string newConstraint = "requires (std::is_integral_v<T>)";
            std::string newSpecialization = "template <typename T> " + newConstraint + " class " + templateName + "<T> { /* specialized behavior */ };";

            if (specSource.find(newConstraint) == std::string::npos) {
                Rewrite.InsertTextAfter(spec->getEndLoc(), "\n/*mut532*/" + newSpecialization);
            }
        }
    }
}

void MutatorFrontendAction_532::MutatorASTConsumer_532::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isTemplateInstantiation()))).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```