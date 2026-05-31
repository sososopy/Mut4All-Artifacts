//source file
#include "../include/insert_virtual_base_class_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if ((!BaseClass->isStruct() && !BaseClass->isClass()) || BaseClass->isLambda())
            return;
        if (!BaseClass->isCompleteDefinition())
            return;
        base_classes.push_back(BaseClass);
    } else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if ((!DerivedClass->isStruct() && !DerivedClass->isClass()) || DerivedClass->isLambda())
            return;
        if (!DerivedClass->isCompleteDefinition())
            return;

        if (base_classes.empty())
            return;

        std::string virtual_base_insertion;
        for (const auto *Base : base_classes) {
            virtual_base_insertion += " : virtual " + Base->getNameAsString();
        }
        
        std::string insertion_text = "class Mut_430" + virtual_base_insertion + " {};\n";
        SourceLocation insert_location = DerivedClass->getEndLoc();
        Rewrite.InsertTextAfterToken(insert_location, "\n/*mut430*/" + insertion_text);
    }
}

void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseClassMatcher = cxxRecordDecl().bind("BaseClass");
    DeclarationMatcher derivedClassMatcher = cxxRecordDecl().bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseClassMatcher, &callback);
    matchFinder.addMatcher(derivedClassMatcher, &callback);
    matchFinder.matchAST(Context);
}