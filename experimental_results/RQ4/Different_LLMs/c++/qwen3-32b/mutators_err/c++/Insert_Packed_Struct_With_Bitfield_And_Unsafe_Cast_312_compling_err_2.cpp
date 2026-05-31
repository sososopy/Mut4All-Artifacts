//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Packed_Struct_With_Bitfield_And_Unsafe_Cast_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Insert_Packed_Struct_With_Bitfield_And_Unsafe_Cast_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;

        if (recordDecl->isUnion() || recordDecl->isLambda())
            return;

        if (!recordDecl->isCompleteDefinition())
            return;

        bool hasPackedAttr = false;
        for (const auto *attr : recordDecl->getAttrs()) {
            if (attr->getKind() == clang::Attr::Packed) {
                hasPackedAttr = true;
                break;
            }
        }
        if (hasPackedAttr)
            return;

        bool hasBitfield = false;
        std::string bitfieldName;
        for (const auto *field : recordDecl->fields()) {
            if (field->isBitField()) {
                hasBitfield = true;
                bitfieldName = field->getNameAsString();
                break;
            }
        }

        if (!hasBitfield) {
            bitfieldName = "mut_bitfield";
            std::string syntheticBitfield = "    unsigned " + bitfieldName + ":1;";
            SourceLocation endLoc = recordDecl->getEndLoc();
            Rewrite.InsertText(endLoc, syntheticBitfield, true, true);
        }

        SourceLocation endLoc = recordDecl->getEndLoc();
        Rewrite.InsertText(endLoc, " __attribute__((packed))", true, true);

        std::string methodCode = "    template <typename T> auto castField() { return (T*)&" + bitfieldName + "; }\n";
        Rewrite.InsertText(endLoc, methodCode, true, true);
    }
}

void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(unless(hasAttr(attr::Packed))).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}