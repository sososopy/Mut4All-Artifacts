```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * alter_template_class_prototype_640
 */ 
class MutatorFrontendAction_640 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(640)

private:
    class MutatorASTConsumer_640 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_640(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingClassNames;
    };
};

//source file
#include "../include/alter_template_class_prototype_640.h"

// ========================================================================================================
#define MUT640_OUTPUT 1

void MutatorFrontendAction_640::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        // Get the source code text of target node
        auto templateDeclText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

        // Extract the template class name and alter it
        std::string newClassName;
        if (!existingClassNames.empty()) {
            newClassName = existingClassNames[getrandom::getRandomIndex(existingClassNames.size() - 1)];
        } else {
            return; // No existing class names to replace with
        }

        // Modify the template declaration
        size_t startPos = templateDeclText.find("class ");
        if (startPos != std::string::npos) {
            size_t endPos = templateDeclText.find('<', startPos);
            if (endPos != std::string::npos) {
                templateDeclText.replace(startPos + 6, endPos - startPos - 6, newClassName);
                size_t conditionStart = templateDeclText.find(',', endPos);
                if (conditionStart != std::string::npos) {
                    size_t conditionEnd = templateDeclText.find('>', conditionStart);
                    if (conditionEnd != std::string::npos) {
                        templateDeclText.erase(conditionStart, conditionEnd - conditionStart);
                    }
                }
            }
        }

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDeclText);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ExistingClasses")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        // Record existing class names
        existingClassNames.push_back(CD->getNameAsString());
    }
}

void MutatorFrontendAction_640::MutatorASTConsumer_640::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto templateMatcher = classTemplateDecl(hasName("Foo")).bind("TemplateDecl");
    auto classMatcher = cxxRecordDecl().bind("ExistingClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}
```