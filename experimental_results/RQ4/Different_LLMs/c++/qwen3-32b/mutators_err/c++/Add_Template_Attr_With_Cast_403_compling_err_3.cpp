//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Attr_With_Cast_403
 */ 
class MutatorFrontendAction_403 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(403)
private:
    class MutatorASTConsumer_403 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_403(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Add_Template_Attr_With_Cast_403.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        const CXXRecordDecl *record = method->getParent();
        if (!record || !record->isTemplated())
            return;

        const ClassTemplateDecl *templateDecl = dyn_cast<ClassTemplateDecl>(record);
        if (!templateDecl)
            return;

        const TemplateParameterList *params = templateDecl->getTemplateParameters();
        if (!params || params->size() == 0)
            return;

        const TemplateTypeParmDecl *tParam = cast<TemplateTypeParmDecl>(params->getParam(0));
        std::string tName = tParam->getNameAsString();

        for (const auto *Attr : method->getAttrs()) {
            SourceRange attrRange = Attr->getRange();
            std::string originalAttrText = stringutils::rangetoStr(*Result.SourceManager, attrRange);
            std::string newAttrText = originalAttrText + "((10, (void)" + tName + "{}))";
            Rewrite.ReplaceText(attrRange, newAttrText);
        }
    }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(clang::ast_matchers::hasAttr(clang::ast_matchers::attr(clang::ast_matchers::anything()))).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}