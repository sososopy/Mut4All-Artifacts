//source file
#include "../include/Replace_Template_Parameter_With_Dependent_Type_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpec")) {
        if (!MT)
            return;

        if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
            if (!SD->isCompleteDefinition())
                return;

            std::string structName = SD->getNameAsString();
            auto templateArgs = MT->template_arguments();
            if (templateArgs.size() > 0) {
                std::string newTemplateParam = "typename T";
                std::string originalType = templateArgs[0].getAsType().getAsString();
                std::string replacement = "__make_integer_seq<" + newTemplateParam + ", int, 42>";

                std::string newStructDecl = "template <" + newTemplateParam + "> struct " + structName + " { " + replacement + " m; };";
                std::string instantiation = structName + "<" + originalType + ">";

                SourceLocation startLoc = SD->getBeginLoc();
                SourceLocation endLoc = SD->getEndLoc().getLocWithOffset(1);
                Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newStructDecl);
                Rewrite.ReplaceText(SD->getSourceRange(), instantiation);
            }
        }
    }
}

void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    TypeMatcher templateSpecMatcher = templateSpecializationType().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(templateSpecMatcher, &callback);
    matchFinder.matchAST(Context);
}