//source file
#include "../include/Modify_Variadic_Template_Parameter_Type_124.h"

// ========================================================================================================
#define MUT124_OUTPUT 1

void MutatorFrontendAction_124::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;
        
        bool typeExists = false;
        for (const auto &param : FT->getTemplateParameters()->asArray()) {
            if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (typeParam->getNameAsString() == "double") {
                    typeExists = true;
                    break;
                }
            }
        }

        if (!typeExists) {
            Rewrite.InsertTextBefore(FT->getBeginLoc(), "using double = double;\n");
        }

        auto functionSource = stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
        std::string oldType = "int";
        std::size_t pos = functionSource.find(oldType);
        if (pos != std::string::npos) {
            functionSource.replace(pos, oldType.length(), "double");
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), functionSource);
    }
}
  
void MutatorFrontendAction_124::MutatorASTConsumer_124::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}