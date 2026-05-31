//header file
#pragma once
#include "Mutator_base.h"

/**
 * Universal_Char_In_Identifier_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(457)

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::string> identifierMap;
    };
};

//source file
#include "../include/universal_char_in_identifier_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;

        std::string originalName = VD->getNameAsString();
        if (originalName.empty())
            return;

        std::string modifiedName = originalName;
        size_t pos = modifiedName.find('V');
        if (pos != std::string::npos) {
            modifiedName.replace(pos, 1, "\\u0056");
            identifierMap[originalName] = modifiedName;

            SourceLocation startLoc = VD->getLocation();
            Rewrite.ReplaceText(startLoc, originalName.length(), modifiedName);
        }
    }

    if (auto *ID = Result.Nodes.getNodeAs<clang::DeclRefExpr>("idRef")) {
        if (!ID || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ID->getLocation()))
            return;

        std::string originalName = ID->getNameInfo().getName().getAsString();
        if (identifierMap.find(originalName) != identifierMap.end()) {
            std::string modifiedName = identifierMap[originalName];
            SourceLocation startLoc = ID->getLocation();
            Rewrite.ReplaceText(startLoc, originalName.length(), modifiedName);
        }
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher varDeclMatcher = varDecl().bind("varDecl");
    StatementMatcher idRefMatcher = declRefExpr().bind("idRef");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.addMatcher(idRefMatcher, &callback);
    matchFinder.matchAST(Context);
}