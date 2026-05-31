//header file
#pragma once
#include "Mutator_base.h"

/**
 * MutatorDefaultTemplateArgInForRangeLoop_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/MutatorDefaultTemplateArgInForRangeLoop_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;

        for (const auto *param : FD->parameters()) {
            if (param->hasDefaultArgument()) {
                QualType paramType = param->getType();
                if (auto ttpType = paramType->getAs<clang::TemplateTypeParmType>()) {
                    std::string paramTypeStr = paramType.getAsString();
                    std::string paramName = param->getNameAsString();

                    if (auto recordType = paramType->getAs<clang::RecordType>()) {
                        clang::CXXRecordDecl *recordDecl = recordType->getDecl();
                        bool hasGetCollection = false;
                        for (const auto *method : recordDecl->methods()) {
                            if (method->getNameAsString() == "getCollection") {
                                hasGetCollection = true;
                                break;
                            }
                        }
                        if (!hasGetCollection) {
                            clang::SourceLocation endLoc = recordDecl->getEndLoc();
                            std::string getCollectionCode = "    auto getCollection() const { return collection; }\n";
                            Rewrite.InsertText(endLoc, getCollectionCode, true, true);
                        }

                        bool hasProcess = false;
                        for (const auto *method : recordDecl->methods()) {
                            if (method->getNameAsString() == "process") {
                                hasProcess = true;
                                break;
                            }
                        }
                        if (!hasProcess) {
                            clang::SourceLocation endLoc = recordDecl->getEndLoc();
                            std::string processCode = "    void process() { }\n";
                            Rewrite.InsertText(endLoc, processCode, true, true);
                        }
                    }

                    std::string loopCode = "for (const " + paramTypeStr + "& elem : " + paramName + ".getCollection()) {\n    elem.process();\n}\n";

                    const clang::CompoundStmt *body = FD->getBody();
                    for (const clang::Stmt *stmt : body->body()) {
                        if (isa<clang::ExprStmt>(stmt)) {
                            clang::SourceRange stmtRange = stmt->getSourceRange();
                            Rewrite.ReplaceText(stmtRange, loopCode);
                            break;
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        hasParameter(0, hasDefaultArgument()),
        hasBody(compoundStmt())
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}