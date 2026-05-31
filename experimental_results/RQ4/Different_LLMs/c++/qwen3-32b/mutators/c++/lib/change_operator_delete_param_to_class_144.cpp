//source file
#include "../include/Change_Operator_Delete_Param_To_Class_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check if the matched node is a CXXRecordDecl (class)
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;
        // Collect the class name
        classNames.push_back(recordDecl->getNameAsString());
    }
    // Check if the matched node is a CXXMethodDecl (operator delete)
    else if (auto *methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("opDelete")) {
        if (!methodDecl || !Result.Context->getSourceManager().isWrittenInMainFile(methodDecl->getLocation()))
            return;
        if (methodDecl->getNameAsString() != "operator delete")
            return;
        // Get the first parameter
        if (methodDecl->getNumParams() < 1)
            return;
        auto *paramDecl = methodDecl->getParamDecl(0);
        if (!paramDecl)
            return;
        auto paramType = paramDecl->getType();
        // Check if the first parameter is a built-in type or a different class
        bool isBuiltIn = true;
        if (paramType->isRecordType()) {
            auto *enclosingClass = methodDecl->getParent();
            auto *recordType = paramType->getAs<clang::RecordType>();
            if (recordType && recordType->getDecl() != enclosingClass) {
                // It's a different class type
                isBuiltIn = false;
            }
        }
        // If it's a built-in type or a different class, proceed
        if (isBuiltIn || (!isBuiltIn && paramType->isRecordType())) {
            // Now, select a class name from the collected list
            if (classNames.empty())
                return;
            std::size_t index = getrandom::getRandomIndex(classNames.size());
            std::string newClassName = classNames[index];
            // Get the source range of the parameter's type
            auto typeSourceInfo = paramDecl->getTypeSourceInfo();
            if (!typeSourceInfo)
                return;
            SourceRange typeRange = typeSourceInfo->getTypeLoc().getSourceRange();
            if (typeRange.isInvalid())
                return;
            // Replace the type in the source code
            Rewrite.ReplaceText(typeRange, newClassName);
        }
    }
}

void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    // Matcher for CXXRecordDecl (classes)
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("recordDecl");
    // Matcher for CXXMethodDecl (operator delete)
    DeclarationMatcher opDeleteMatcher = cxxMethodDecl(
        hasName("operator delete")
    ).bind("opDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(opDeleteMatcher, &callback);
    matchFinder.matchAST(Context);
}