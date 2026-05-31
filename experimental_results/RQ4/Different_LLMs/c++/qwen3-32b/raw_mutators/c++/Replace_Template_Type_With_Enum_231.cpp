//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_With_Enum_231
 */ 
class MutatorFrontendAction_231 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(231)

private:
    class MutatorASTConsumer_231 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_231(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> enumNames;
    };
};

//source file
#include "../include/Replace_Template_Type_With_Enum_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *enumDecl = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!enumDecl || !Result.Context->getSourceManager().isWrittenInMainFile(enumDecl->getLocation()))
            return;
        enumNames.push_back(enumDecl->getNameAsString());
    } 
    else if (auto *templateSpec = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("templateSpec")) {
        if (!templateSpec || !Result.Context->getSourceManager().isWrittenInMainFile(templateSpec->getLocation()))
            return;
        
        auto templateDecl = templateSpec->getSpecializedTemplate();
        if (auto *classTemplate = dyn_cast<clang::ClassTemplateDecl>(templateDecl)) {
            std::string templateName = classTemplate->getNameAsString();
            if (templateName == "is_class" || templateName == "is_pod" || templateName == "is_standard_layout") {
                if (!enumNames.empty()) {
                    clang::TemplateArgumentListInfo *args = templateSpec->getTemplateArgs();
                    if (args && args->size() > 0) {
                        clang::TemplateArgumentLoc argLoc = args->get(0);
                        SourceLocation startLoc = argLoc.getSourceRange().getBegin();
                        SourceLocation endLoc = argLoc.getSourceRange().getEnd();
                        std::string enumName = enumNames[0];
                        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), enumName);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("enumDecl");
    DeclarationMatcher templateMatcher = classTemplateSpecializationDecl().bind("templateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}