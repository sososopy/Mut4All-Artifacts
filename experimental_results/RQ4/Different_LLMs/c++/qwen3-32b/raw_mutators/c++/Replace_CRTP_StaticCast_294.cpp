//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_CRTP_StaticCast_294
 */ 
class MutatorFrontendAction_294 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(294)

private:
    class MutatorASTConsumer_294 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_294(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_CRTP_StaticCast_294.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

using namespace clang;
using namespace clang::ast_matchers;

namespace {
bool isCRTPBase(const CXXRecordDecl *RD) {
    if (!RD->isClassTemplateSpecialization())
        return false;

    const TemplateArgument &TA = RD->getTemplateArgs().getArg(0);
    if (TA.getKind() != TemplateArgument::Type)
        return false;

    const Type *T = TA.getAsType();
    if (!T->getAs<RecordType>())
        return false;

    const RecordType *RT = T->getAs<RecordType>();
    return RD == RT->getDecl();
}

bool isThisExpr(const Expr *E) {
    if (!E)
        return false;
    const auto *DRE = dyn_cast<DeclRefExpr>(E);
    return DRE && DRE->isThis();
}
}

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<CallExpr>("callExpr")) {
        // Filter nodes in header files
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        // Check if in a consteval method
        const CXXMethodDecl *MD = dyn_cast<CXXMethodDecl>(CE->getImmediateFunction());
        if (!MD || !MD->isConsteval())
            return;

        // Check if method's class is CRTP base
        const CXXRecordDecl *RD = MD->getParent();
        if (!isCRTPBase(RD))
            return;

        // Check first argument is static_cast<Derived>(*this)
        const Expr *Arg0 = CE->getArg(0)->IgnoreImpCasts();
        const CXXStaticCastExpr *SCast = dyn_cast<CXXStaticCastExpr>(Arg0);
        if (!SCast)
            return;

        // Check source is *this
        const Expr *SubExpr = SCast->getSubExpr()->IgnoreImpCasts();
        if (!isThisExpr(SubExpr))
            return;

        // Check destination type is template parameter of CRTP base
        const Type *DestType = SCast->getType().getTypePtr();
        const TemplateTypeParmType *TTP = DestType->getAs<TemplateTypeParmType>();
        if (!TTP || TTP->getDepth() != 0 || TTP->getIndex() != 0)
            return;

        // Perform mutation
        SourceRange castRange = SCast->getSourceRange();
        SourceLocation castStart = castRange.getBegin();
        SourceLocation castEnd = castRange.getEnd();

        // Get original text
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, castRange);
        std::string newCast = originalText;
        
        // Replace 'static_cast<Derived>' with 'static_cast<Derived*'
        size_t castPos = newCast.find("static_cast<");
        if (castPos != std::string::npos) {
            newCast.replace(castPos, 12, "static_cast<Derived*");
        }
        
        // Replace '(*this' with 'this'
        size_t thisPos = newCast.find("(*this");
        if (thisPos != std::string::npos) {
            newCast.replace(thisPos, 6, "this");
        }
        
        // Replace '.' with '->'
        size_t dotPos = newCast.find('.');
        if (dotPos != std::string::npos) {
            newCast.replace(dotPos, 1, "->");
        }

        // Apply replacement
        Rewrite.ReplaceText(castRange, newCast);
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = callExpr(
        hasAncestor(cxxMethodDecl(isConsteval())),
        hasArgument(0, cxxStaticCastExpr(
            hasSourceExpression(ignoringImpCasts(declRefExpr(to(thisExpr()))))
        ))
    ).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}