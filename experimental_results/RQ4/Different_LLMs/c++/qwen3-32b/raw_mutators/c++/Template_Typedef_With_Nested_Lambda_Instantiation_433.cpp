//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Typedef_With_Nested_Lambda_Instantiation_433
 */ 
class MutatorFrontendAction_433 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(433)

private:
    class MutatorASTConsumer_433 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_433(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_Template_Typedef_With_Nested_Lambda_Instantiation_433.h"

// ========================================================================================================
#define MUT433_OUTPUT 1

void MutatorFrontendAction_433::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        // Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        // Check if it's not already a template
        if (RD->getDescribedClassTemplate()) {
            return;
        }

        // Get the source code of the struct
        auto originalCode = stringutils::rangetoStr(*Result.SourceManager, RD->getSourceRange());
        if (originalCode.empty()) return;

        // Find the closing brace
        size_t pos = originalCode.rfind('}');
        if (pos == std::string::npos) return;

        // Insert the nested template alias before the closing brace
        std::string newCode = originalCode;
        newCode.insert(pos, "\n  template <typename T>\n  using Type = decltype(F.template operator()<T>(0));\n");

        // Add the template header
        newCode = "template <auto F>\n" + newCode;

        // Replace the original struct with the new code
        Rewrite.ReplaceText(RD->getSourceRange(), newCode);

        // Insert the lambda and using declaration at the end of the main file
        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation endLoc = SM.getLocForEndOfFile(SM.getMainFileID());
        std::string insertionText = "\n\nauto lambda = [](auto x) { return x; };\nusing T = Foo<lambda>::Type<int>;\n";
        Rewrite.InsertText(endLoc, insertionText, true, true);
    }
}
  
void MutatorFrontendAction_433::MutatorASTConsumer_433::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}