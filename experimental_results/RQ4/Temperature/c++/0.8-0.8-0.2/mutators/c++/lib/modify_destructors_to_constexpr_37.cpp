//source file
#include "../include/modify_destructors_to_constexpr_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        // Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
          return;

        // Ensure the class is a complete definition
        if (!CL->isCompleteDefinition())
          return;

        CurrentClass = CL;
        auto Destr = CL->getDestructor();
        std::string modification;

        if (Destr) {
            // Check if destructor is user-defined
            if (Destr->isUserProvided()) {
                // Get source code text of target node
                auto destructorSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                                Destr->getSourceRange());
                // Perform mutation on the source code text
                modification = "constexpr ~" + CL->getNameAsString() + "() = default;\n";
            } else if (!Destr->isImplicit() && !Destr->isTrivial()) {
                auto destructorSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                                Destr->getSourceRange());
                modification = "constexpr " + destructorSource + "\n";
            }
        } else {
            // If no destructor defined, add a constexpr default destructor
            modification = "public: constexpr ~" + CL->getNameAsString() + "() = default;\n";
        }

        // Replace the original AST node with the mutated one
        if (!modification.empty()) {
            Rewrite.ReplaceText(CL->getEndLoc(), 0, "/*mut37*/" + modification);
        }
    }
}

void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}