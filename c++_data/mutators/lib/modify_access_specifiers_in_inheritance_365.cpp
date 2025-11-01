//source file
#include "../include/modify_access_specifiers_in_inheritance_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (const auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        // Filter nodes in header files
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;

        // Iterate over base classes and modify access specifier
        for (const auto &Base : DerivedClass->bases()) {
            if (Base.getAccessSpecifier() == clang::AS_private) {
                // Perform mutation on the access specifier
                std::string replacement = "protected";
                SourceLocation StartLoc = Base.getBeginLoc();
                SourceLocation EndLoc = Base.getEndLoc();
                Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), replacement);
                break; // Modify only one base class
            }
        }
    }
}

void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isDerivedFrom(anything()), hasDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}