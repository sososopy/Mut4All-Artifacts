//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * Template_Parameter_Type_Mismatch_391
 */ 
class MutatorFrontendAction_391 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(391)

private:
    class MutatorASTConsumer_391 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_391(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateDecl *TemplateDeclNode = nullptr;
        const clang::NamedDecl *TemplateNameNode = nullptr;
    };
};

//source file
#include "../include/template_parameter_type_mismatch_391.h"

// ========================================================================================================
#define MUT391_OUTPUT 1

void MutatorFrontendAction_391::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;
        TemplateDeclNode = TD;
    } else if (auto *TN = Result.Nodes.getNodeAs<clang::NamedDecl>("TemplateName")) {
        if (!TN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TN->getLocation()))
            return;
        TemplateNameNode = TN;
    }

    if (TemplateDeclNode && TemplateNameNode) {
        //Get the source code text of target node
        auto templateDeclText = stringutils::rangetoStr(*(Result.SourceManager), TemplateDeclNode->getSourceRange());
        auto templateNameText = stringutils::rangetoStr(*(Result.SourceManager), TemplateNameNode->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string newTypeName = "C";
        std::string newTypeDef = "struct " + newTypeName + " { /* intentionally differs */ };";

        //Insert the new type definition before the template declaration
        Rewrite.InsertTextBefore(TemplateDeclNode->getBeginLoc(), newTypeDef + "\n");

        //Replace the template instantiation with a mismatched type
        std::string newInstantiation = templateNameText;
        size_t pos = newInstantiation.find('<');
        if (pos != std::string::npos) {
            newInstantiation.replace(pos + 1, std::string::npos, newTypeName + ">");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TemplateNameNode->getSourceRange()), newInstantiation);
        }
    }
}
  
void MutatorFrontendAction_391::MutatorASTConsumer_391::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation.
    auto templateMatcher = templateDecl().bind("TemplateDecl");
    auto templateNameMatcher = namedDecl().bind("TemplateName");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(templateNameMatcher, &callback);
    matchFinder.matchAST(Context);
}