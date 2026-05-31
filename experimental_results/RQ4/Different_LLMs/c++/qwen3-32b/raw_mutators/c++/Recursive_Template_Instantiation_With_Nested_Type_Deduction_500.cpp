//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Instantiation_With_Nested_Type_Deduction_500
 */ 
class MutatorFrontendAction_500 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(500)

private:
    class MutatorASTConsumer_500 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_500(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Recursive_Template_Instantiation_With_Nested_Type_Deduction_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        // Check if the class has a NestedType
        bool hasNestedType = false;
        for (const auto *D : RD->decls()) {
            if (const auto *UD = dyn_cast<clang::UsingDecl>(D)) {
                if (UD->getNameAsString() == "NestedType") {
                    hasNestedType = true;
                    break;
                }
            }
        }
        if (!hasNestedType) {
            // Insert using NestedType = int;
            SourceLocation endLoc = RD->getEndLoc();
            Rewrite.InsertText(endLoc, "\n    using NestedType = int;", true, true);
        }
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplate")) {
        // Check if the function's body is a return new T
        const auto *FD = FTD->getTemplatedDecl();
        if (!FD || !FD->hasBody())
            return;
        const auto *Body = FD->getBody();
        if (!Body)
            return;
        // Check if the body is a return statement with new T
        std::string bodyText = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());
        size_t pos = bodyText.find("return new T;");
        if (pos != std::string::npos) {
            // Replace with return new T(factory<decltype(T::NestedType)>());
            std::string mutatedBody = bodyText;
            mutatedBody.replace(pos, 13, "return new T(factory<decltype(T::NestedType)>());");
            // Replace the original body with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedBody);
        }
    }
}

void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match CXXRecordDecls to add NestedType if missing
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("recordDecl");
    // Match function templates that return new T
    DeclarationMatcher functionMatcher = functionTemplateDecl().bind("functionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}