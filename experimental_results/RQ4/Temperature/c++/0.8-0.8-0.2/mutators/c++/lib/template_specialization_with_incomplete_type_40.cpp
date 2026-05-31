//source file
#include "../include/template_specialization_with_incomplete_type_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FunctionTemplate = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FunctionTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(FunctionTemplate->getLocation()))
            return;

        const TemplateParameterList *Params = FunctionTemplate->getTemplateParameters();
        if (Params->size() != 1)
            return;

        const TemplateTypeParmDecl *TypeParam = dyn_cast<TemplateTypeParmDecl>(Params->getParam(0));
        if (!TypeParam || !TypeParam->isParameterPack())
            return;

        std::string TemplateName = FunctionTemplate->getNameAsString();
        std::string MutatedCode = "struct UndefinedType; \ntemplate<> void " + TemplateName + "<UndefinedType>(" + TemplateName + "<const UndefinedType> p);";
        
        SourceLocation InsertLocation = FunctionTemplate->getEndLoc().getLocWithOffset(1); 
        Rewrite.InsertText(InsertLocation, "\n/*mut40*/" + MutatedCode);
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}