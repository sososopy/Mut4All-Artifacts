//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Initializer_With_Template_89
 */ 
class MutatorFrontendAction_89 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(89)

private:
    class MutatorASTConsumer_89 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_89(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Initializer_With_Template_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("field")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->hasInit())
            return;

        QualType TType = FD->getType();
        PrintingPolicy Policy = Result.Context->getPrintingPolicy();
        std::string TName = TType.getAsString(Policy);

        std::string newInit = "typename std::conditional<std::is_integral<" + TName + ">::value, A, B>::type()";
        std::string newDecl = TName + " " + FD->getNameAsString() + " {" + newInit + "};";

        Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
    }
}
  
void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl().bind("field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);

    bool hasA = false;
    bool hasB = false;
    for (const auto &Decl : Context.getTranslationUnitDecl()->decls()) {
        if (const TypedefDecl *TD = dyn_cast<TypedefDecl>(Decl)) {
            if (TD->getNameAsString() == "A") hasA = true;
            else if (TD->getNameAsString() == "B") hasB = true;
        }
    }

    if (!hasA) {
        SourceLocation beginLoc = Context.getSourceManager().getLocForStartOfFile(
            Context.getSourceManager().getMainFileID());
        TheRewriter.InsertText(beginLoc, "typedef int A;\n", true, true);
    }
    if (!hasB) {
        SourceLocation beginLoc = Context.getSourceManager().getLocForStartOfFile(
            Context.getSourceManager().getMainFileID());
        TheRewriter.InsertText(beginLoc, "typedef int B;\n", true, true);
    }
}