```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Usage_Within_Structs_437
 */ 
class MutatorFrontendAction_437 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(437)

private:
    class MutatorASTConsumer_437 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_437(Rewriter &R) : TheRewriter(R) {}
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
        const clang::EnumDecl *EnumDeclNode = nullptr;
    };
};

//source file
#include "../include/Enum_Usage_Within_Structs_437.h"

// ========================================================================================================
#define MUT437_OUTPUT 1

void MutatorFrontendAction_437::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructWithEnum")) {
        //Filter nodes in header files
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;

        for (const auto *D : SD->decls()) {
            if (const auto *ED = llvm::dyn_cast<clang::EnumDecl>(D)) {
                if (ED->enumerator_begin() != ED->enumerator_end()) {
                    EnumDeclNode = ED;
                    break;
                }
            }
        }
    }

    if (EnumDeclNode) {
        // Get the source code text of target node
        auto enumName = EnumDeclNode->getNameAsString();
        std::string usingEnumText = "using enum " + enumName + ";\n";
        
        // Perform mutation on the source code text by applying string replacement
        std::string compoundExpression = "int result = (";
        bool first = true;
        for (const auto *E : EnumDeclNode->enumerators()) {
            if (!first) {
                compoundExpression += " | ";
            }
            compoundExpression += E->getNameAsString();
            first = false;
        }
        compoundExpression += ");\n";

        // Replace the original AST node with the mutated one
        auto insertLoc = Result.Context->getTranslationUnitDecl()->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "/*mut437*/\n" + usingEnumText + compoundExpression);
    }
}
  
void MutatorFrontendAction_437::MutatorASTConsumer_437::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(enumDecl())).bind("StructWithEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```