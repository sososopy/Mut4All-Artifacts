//source file
#include "../include/template_base_class_method_lookup_170.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_170::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;

        if (MT->isDependentContext()) {
            const auto *Parent = dyn_cast<CXXRecordDecl>(MT->getParent());
            if (Parent && Parent->getDescribedClassTemplate()) {
                for (const auto &Base : Parent->bases()) {
                    if (const auto *BaseType = Base.getType()->getAs<TemplateSpecializationType>()) {
                        std::string baseName = BaseType->getTemplateName().getAsTemplateDecl()->getNameAsString();
                        std::string methodName = MT->getNameAsString();
                        std::string replacement = "(this->*static_cast<" + MT->getReturnType().getAsString() +
                                                  " (" + baseName + "<T>::*)() const>(&" + baseName + "<T>::" + methodName + "))()";
                        SourceRange methodRange = MT->getBody()->getSourceRange();
                        std::string methodBody = stringutils::rangetoStr(*(Result.SourceManager), methodRange);
                        size_t pos = methodBody.find("this->" + methodName);
                        if (pos != std::string::npos) {
                            methodBody.replace(pos, methodName.length() + 6, replacement);
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(methodRange), methodBody);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_170::MutatorASTConsumer_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(ofClass(cxxRecordDecl(isTemplateInstantiation()))).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}