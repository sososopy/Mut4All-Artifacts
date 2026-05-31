//source file
#include "../include/modify_template_instantiation_in_class_hierarchies_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->hasDefinition() || CL->isLambda())
            return;

        targetClasses.push_back(CL);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->hasDefinition() || CL->isLambda())
            return;

        for (const auto &Base : CL->bases()) {
            if (const auto *BaseType = Base.getType()->getAsCXXRecordDecl()) {
                if (std::find(targetClasses.begin(), targetClasses.end(), BaseType) != targetClasses.end()) {
                    std::string intermediateName = "Intermediate_" + BaseType->getNameAsString();
                    std::string intermediateDef = "class " + intermediateName + " : public " + BaseType->getNameAsString() + " {};\n";
                    std::string newBase = "public " + intermediateName;

                    SourceLocation insertLoc = BaseType->getEndLoc();
                    Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut389*/" + intermediateDef);

                    SourceLocation replaceStart = Base.getSourceRange().getBegin();
                    SourceLocation replaceEnd = Base.getSourceRange().getEnd();
                    Rewrite.ReplaceText(SourceRange(replaceStart, replaceEnd), newBase);
                }
            }
        }
    }
}

void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl(isTemplateInstantiation())))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}