//source file
#include "../include/insert_virtual_base_class_82.h"

// ========================================================================================================
#define MUT82_OUTPUT 1

void MutatorFrontendAction_82::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;

        std::string className = CL->getNameAsString();
        if (processedClasses.find(className) != processedClasses.end())
            return;

        processedClasses.insert(className);

        std::string baseClassName = "VirtualBase_" + className;
        std::string baseClassDef = "class " + baseClassName + " {};";
        std::string derivedClassDef = "class " + className + " : public virtual " + baseClassName + " {};\n";

        SourceLocation insertLoc = CL->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, baseClassDef + "\n");
        Rewrite.InsertTextAfterToken(CL->getEndLoc(), derivedClassDef);
    }
}

void MutatorFrontendAction_82::MutatorASTConsumer_82::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}