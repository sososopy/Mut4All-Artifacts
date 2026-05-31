//header file
#pragma once
#include "Mutator_base.h"

/**
 * replace_enum_base_type_in_templated_struct_536
 */ 
class MutatorFrontendAction_536 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(536)

private:
    class MutatorASTConsumer_536 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_536(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Enum_Base_Type_In_Templated_Struct_536.h"

// ========================================================================================================
#define MUT536_OUTPUT 1

void MutatorFrontendAction_536::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        // Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                ED->getLocation()))
            return;

        // Check if the enum is within a templated struct
        if (auto *RD = dyn_cast<CXXRecordDecl>(ED->getDeclContext())) {
            if (RD->getDescribedClassTemplate()) {
                // Perform mutation on the source code text by applying string replacement
                if (ED->isScoped()) {
                    auto baseType = ED->getIntegerType();
                    if (baseType->isDependentType()) {
                        // Replace the templated type with a concrete type
                        std::string enumText = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
                        size_t pos = enumText.find(": " + baseType.getAsString());
                        if (pos != std::string::npos) {
                            enumText.replace(pos, baseType.getAsString().length() + 2, ": int");
                            // Replace the original AST node with the mutated one
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumText);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_536::MutatorASTConsumer_536::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = enumDecl(isScoped(), hasDeclContext(cxxRecordDecl(has(cxxRecordDecl(isTemplateInstantiation()))))).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}