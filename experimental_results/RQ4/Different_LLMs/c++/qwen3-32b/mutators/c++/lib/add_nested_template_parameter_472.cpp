//source file
#include "../include/Add_Nested_Template_Parameter_472.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/Basic/SourceLocation.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        // Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        const clang::DeclContext *DC = FT->getDeclContext();
        if (auto *ClassDecl = dyn_cast<clang::CXXRecordDecl>(DC)) {
            if (auto *ClassTemplate = ClassDecl->getDescribedClassTemplate()) {
                clang::TemplateParameterList *ClassTPL = ClassTemplate->getTemplateParameters();
                if (ClassTPL->size() < 1)
                    return;
                auto *TDecl = ClassTPL->getParam(0);
                clang::TemplateTypeParmDecl *TTParam = dyn_cast<clang::TemplateTypeParmDecl>(TDecl);
                if (!TTParam)
                    return;
                std::string TName = TTParam->getNameAsString();

                clang::TemplateParameterList *FuncTPL = FT->getTemplateParameters();
                if (FuncTPL->size() < 1)
                    return;
                auto *UDecl = FuncTPL->getParam(0);
                clang::TemplateTypeParmDecl *UTParam = dyn_cast<clang::TemplateTypeParmDecl>(UDecl);
                if (!UTParam)
                    return;
                std::string UName = UTParam->getNameAsString();

                // Get the source code of the function's template declaration
                SourceManager &SM = *Result.SourceManager;
                auto sourceRange = FT->getSourceRange();
                if (sourceRange.isInvalid())
                    return;

                std::string originalTemplate = stringutils::rangetoStr(SM, sourceRange);

                // Find the '>' to replace
                size_t pos = originalTemplate.find('>');
                if (pos == std::string::npos)
                    return;

                std::string newParams = ", typename V = std::vector<" + TName + ">, typename W = std::pair<V, " + UName + ">>";
                std::string newTemplate = originalTemplate.substr(0, pos) + newParams;

                // Apply the mutation
                Rewrite.ReplaceText(sourceRange, newTemplate);
            }
        }
    }
}

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify function templates inside class templates
    DeclarationMatcher matcher = functionTemplateDecl(hasDeclContext(classTemplateDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}