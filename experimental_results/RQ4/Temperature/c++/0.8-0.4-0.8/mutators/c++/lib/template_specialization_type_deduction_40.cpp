//source file
#include "../include/template_specialization_type_deduction_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate) {
            auto *TemplateParams = FD->getDescribedFunctionTemplate()->getTemplateParameters();
            if (TemplateParams->size() == 1) {
                if (auto *ParamType = dyn_cast<TemplateTypeParmDecl>(TemplateParams->getParam(0))) {
                    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
                        auto ArgType = CallExpr->getArg(0)->getType();
                        if (ArgType.isConstQualified()) {
                            std::string newType = "Nested<const " + ArgType.getAsString() + ">";
                            std::string newDecl = "template<class T> struct Nested {};\n";
                            newDecl += "template<class T> struct Container: public Nested<const T> {};\n";
                            newDecl += FD->getReturnType().getAsString() + " " + FD->getNameAsString() + "(Container<const T> c);";
                            Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut40*/" + newDecl + "\n");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionDecl(hasDescendant(callExpr(hasArgument(0, hasType(isConstQualified()))))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}