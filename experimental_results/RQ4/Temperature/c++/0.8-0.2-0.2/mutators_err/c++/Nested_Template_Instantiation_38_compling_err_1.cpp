```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * nested_template_instantiation_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/nested_template_instantiation_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
        //Filter nodes in header files
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                    CTD->getLocation()))
            return;

        // Get the source code text of target node
        auto classTemplateText = stringutils::rangetoStr(*(Result.SourceManager), CTD->getSourceRange());

        // Check if there's a nested template instantiation within a member function or constructor
        if (classTemplateText.find("Nested") != std::string::npos) {
            // Introduce a new template parameter 'Z'
            std::string::size_type pos = classTemplateText.find("<typename");
            if (pos != std::string::npos) {
                classTemplateText.insert(pos + 9, ", typename Z");
            }

            // Modify the nested template instantiation to use 'Z' and omit 'template' keyword
            std::string::size_type nestedPos = classTemplateText.find("Nested<");
            if (nestedPos != std::string::npos) {
                classTemplateText.replace(nestedPos + 7, 1, "Z");
                std::string::size_type applyPos = classTemplateText.find("::apply", nestedPos);
                if (applyPos != std::string::npos) {
                    classTemplateText.insert(applyPos + 6, "<int>");
                }
            }

            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTD->getSourceRange()), classTemplateText);
        }
    }
}

void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasDescendant(functionDecl())).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```