```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_specialization_and_function_signature_interaction_616
 */ 
class MutatorFrontendAction_616 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(616)

private:
    class MutatorASTConsumer_616 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_616(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_specialization_and_function_signature_interaction_616.h"

// ========================================================================================================
#define MUT616_OUTPUT 1

void MutatorFrontendAction_616::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Spec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("SpecializedTemplate")) {
        if (!Spec || !Result.Context->getSourceManager().isWrittenInMainFile(Spec->getLocation()))
            return;

        std::string newFunction = R"(
        template<typename T, typename U = decltype(process(std::declval<Container<1>>(), nullptr))>
        friend auto process(const Container<0>&, T, U = 0);
        )";
        
        SourceLocation insertLocation = Spec->getEndLoc().getLocWithOffset(-1);
        Rewrite.InsertText(insertLocation, newFunction, true, true);
    }
}

void MutatorFrontendAction_616::MutatorASTConsumer_616::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasName("Container"),
        hasTemplateArgument(0, equals(0))
    ).bind("SpecializedTemplate");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```