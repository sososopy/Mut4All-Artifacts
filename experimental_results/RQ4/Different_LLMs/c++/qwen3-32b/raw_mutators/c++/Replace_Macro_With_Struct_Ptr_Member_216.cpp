//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Macro_With_Struct_Ptr_Member_216
 */

class MutatorFrontendAction_216 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(216)
private:
    class MutatorASTConsumer_216 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_216(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, ASTContext *Context) : Rewrite(Rewrite), Context(Context) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        ASTContext *Context;
        std::vector<const CXXRecordDecl*> structs;
        std::vector<const VarDecl*> structPtrs;
    };
};

//source file
#include "../include/Replace_Macro_With_Struct_Ptr_Member_216.h"

// ========================================================================================================
#define MUT216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ME = Result.Nodes.getNodeAs<clang::MacroExpansion>(Result.Nodes.getMap().find("macro")->second)) {
        if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(ME->getLocation()))
            return;

        SourceManager &SM = *Result.Context->getSourceManager();
        SourceLocation StartLoc = ME->getLocation();
        SourceLocation EndLoc = ME->getSourceRange().getEnd();
        std::string macroText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(StartLoc, EndLoc));

        if (structs.empty() || structPtrs.empty())
            return;

        const CXXRecordDecl *Struct = structs[0];
        const VarDecl *Ptr = structPtrs[0];

        const FieldDecl *Field = Struct->getFirstField();
        if (!Field)
            return;

        size_t openParen = macroText.find('(');
        size_t closeParen = macroText.rfind(')');
        if (openParen == std::string::npos || closeParen == std::string::npos)
            return;

        std::string argsStr = macroText.substr(openParen + 1, closeParen - openParen - 1);
        std::vector<std::string> args = stringutils::split(argsStr, ',');

        if (args.size() < 2)
            return;

        std::string arg1 = stringutils::trim(args[0]);
        std::string arg2 = stringutils::trim(args[1]);

        std::string structMemberAccess = Ptr->getNameAsString() + "->" + Field->getNameAsString();
        std::string replacement = "(" + arg1 + " < " + structMemberAccess + " ? " + arg1 + " : " + structMemberAccess + ")";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ME->getSourceRange()), replacement);
    }
    else if (auto *Struct = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
        if (!Struct || !Result.Context->getSourceManager().isWrittenInMainFile(Struct->getLocation()))
            return;
        structs.push_back(Struct);
    }
    else if (auto *Ptr = Result.Nodes.getNodeAs<clang::VarDecl>("ptrDecl")) {
        if (!Ptr || !Result.Context->getSourceManager().isWrittenInMainFile(Ptr->getLocation()))
            return;
        structPtrs.push_back(Ptr);
    }
}

void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    DeclarationMatcher macroMatcher = macroExpansion(
        hasAncestor(varDecl(isConstexpr()))
    ).bind("macro");

    DeclarationMatcher structMatcher = cxxRecordDecl().bind("structDecl");
    DeclarationMatcher ptrMatcher = varDecl(
        hasType(pointsTo(recordType()))
    ).bind("ptrDecl");

    Callback callback(TheRewriter, &Context);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(ptrMatcher, &callback);
    matchFinder.matchAST(Context);
}