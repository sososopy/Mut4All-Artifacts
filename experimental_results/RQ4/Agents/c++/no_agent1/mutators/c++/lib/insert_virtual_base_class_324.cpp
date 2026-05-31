//source file
#include "../include/insert_virtual_base_class_324.h"

// ========================================================================================================
#define MUT324_OUTPUT 1

void MutatorFrontendAction_324::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;

        if (!recordDecl->isCompleteDefinition() || recordDecl->isLambda())
            return;

        std::string className = recordDecl->getNameAsString();
        if (className.empty() || visitedClasses.find(className) != visitedClasses.end())
            return;

        visitedClasses.insert(className);

        std::string virtualBaseClass = "class VirtualBase_324 {};\n";
        std::string derivedClass = "class DerivedFromVirtualBase_324 : virtual VirtualBase_324, public " + className + " {};\n";
        SourceLocation insertLoc = recordDecl->getEndLoc();

        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut324*/" + virtualBaseClass + derivedClass);
    }
}

void MutatorFrontendAction_324::MutatorASTConsumer_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}