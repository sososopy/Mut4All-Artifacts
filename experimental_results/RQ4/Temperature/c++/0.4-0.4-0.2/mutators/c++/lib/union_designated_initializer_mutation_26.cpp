//source file
#include "../include/Mutator_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (unionDecl) {
            std::string unionName = unionDecl->getNameAsString();
            std::string initCode = "new " + unionName + "{";
            bool first = true;
            for (const auto *field : unionDecl->fields()) {
                if (!first) {
                    initCode += ", ";
                }
                initCode += "." + field->getNameAsString() + " = ";
                if (field->getType()->isIntegerType()) {
                    initCode += "0";
                } else if (field->getType()->isFloatingType()) {
                    initCode += "0.0";
                } else {
                    continue;
                }
                first = false;
            }
            initCode += "};\n";

            SourceLocation insertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
            Rewrite.InsertText(insertLoc, "/*mut26*/" + initCode);
        }
    } else if (auto *UD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Union")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UD->getLocation()))
            return;
        if (UD->isUnion()) {
            unionDecl = UD;
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    auto unionMatcher = cxxRecordDecl(isUnion()).bind("Union");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.matchAST(Context);
}